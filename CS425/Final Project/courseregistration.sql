PGDMP         9                 y            postgres    11.12    11.12     .           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                       false            /           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                       false            0           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                       false            1           1262    13012    postgres    DATABASE     ?   CREATE DATABASE postgres WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'Chinese (Simplified)_China.936' LC_CTYPE = 'Chinese (Simplified)_China.936';
    DROP DATABASE postgres;
             postgres    false            2           0    0    DATABASE postgres    COMMENT     N   COMMENT ON DATABASE postgres IS 'default administrative connection database';
                  postgres    false    2865                        3079    16384 	   adminpack 	   EXTENSION     A   CREATE EXTENSION IF NOT EXISTS adminpack WITH SCHEMA pg_catalog;
    DROP EXTENSION adminpack;
                  false            3           0    0    EXTENSION adminpack    COMMENT     M   COMMENT ON EXTENSION adminpack IS 'administrative functions for PostgreSQL';
                       false    1            ?            1259    16434    Registration    TABLE     ?   CREATE TABLE public."Registration" (
    registrationid integer NOT NULL,
    type character varying NOT NULL,
    studentid integer NOT NULL,
    sectionid integer NOT NULL
);
 "   DROP TABLE public."Registration";
       public         postgres    false            ?            1259    16415    college    TABLE     ?   CREATE TABLE public.college (
    collegeid integer NOT NULL,
    collegename character varying NOT NULL,
    address character varying NOT NULL
);
    DROP TABLE public.college;
       public         postgres    false            ?            1259    16513    course    TABLE     ?   CREATE TABLE public.course (
    courseid integer NOT NULL,
    credit integer NOT NULL,
    coursename character varying(50) NOT NULL,
    hour integer NOT NULL
);
    DROP TABLE public.course;
       public         postgres    false            ?            1259    16535 
   department    TABLE     ?   CREATE TABLE public.department (
    departmentid character varying(20) NOT NULL,
    deptname character varying(50) NOT NULL,
    collegeid integer NOT NULL
);
    DROP TABLE public.department;
       public         postgres    false            ?            1259    16447 
   instructor    TABLE     ?   CREATE TABLE public.instructor (
    instructorid integer NOT NULL,
    name character varying(20) NOT NULL,
    age integer NOT NULL,
    departmentid character varying(20) NOT NULL
);
    DROP TABLE public.instructor;
       public         postgres    false            ?            1259    16423    section    TABLE     ?   CREATE TABLE public.section (
    sectionid integer NOT NULL,
    classroom character varying NOT NULL,
    "date/time" character varying NOT NULL,
    courseid integer NOT NULL,
    instructorid integer NOT NULL
);
    DROP TABLE public.section;
       public         postgres    false            ?            1259    16530    student    TABLE     ?   CREATE TABLE public.student (
    studentid integer NOT NULL,
    name character varying(50) NOT NULL,
    age integer NOT NULL
);
    DROP TABLE public.student;
       public         postgres    false            ?            1259    16540    student2    TABLE     ?   CREATE TABLE public.student2 (
    studentid integer NOT NULL,
    name character varying(50) NOT NULL,
    age integer NOT NULL
);
    DROP TABLE public.student2;
       public         postgres    false            &          0    16434    Registration 
   TABLE DATA               T   COPY public."Registration" (registrationid, type, studentid, sectionid) FROM stdin;
    public       postgres    false    199   ?       $          0    16415    college 
   TABLE DATA               B   COPY public.college (collegeid, collegename, address) FROM stdin;
    public       postgres    false    197   y        (          0    16513    course 
   TABLE DATA               D   COPY public.course (courseid, credit, coursename, hour) FROM stdin;
    public       postgres    false    201   ?        *          0    16535 
   department 
   TABLE DATA               G   COPY public.department (departmentid, deptname, collegeid) FROM stdin;
    public       postgres    false    203   ?!       '          0    16447 
   instructor 
   TABLE DATA               K   COPY public.instructor (instructorid, name, age, departmentid) FROM stdin;
    public       postgres    false    200   p"       %          0    16423    section 
   TABLE DATA               \   COPY public.section (sectionid, classroom, "date/time", courseid, instructorid) FROM stdin;
    public       postgres    false    198   #       )          0    16530    student 
   TABLE DATA               7   COPY public.student (studentid, name, age) FROM stdin;
    public       postgres    false    202   ?#       +          0    16540    student2 
   TABLE DATA               8   COPY public.student2 (studentid, name, age) FROM stdin;
    public       postgres    false    204    $       ?
           2606    16422    college College_pkey 
   CONSTRAINT     [   ALTER TABLE ONLY public.college
    ADD CONSTRAINT "College_pkey" PRIMARY KEY (collegeid);
 @   ALTER TABLE ONLY public.college DROP CONSTRAINT "College_pkey";
       public         postgres    false    197            ?
           2606    16451    instructor Instructor_pkey 
   CONSTRAINT     d   ALTER TABLE ONLY public.instructor
    ADD CONSTRAINT "Instructor_pkey" PRIMARY KEY (instructorid);
 F   ALTER TABLE ONLY public.instructor DROP CONSTRAINT "Instructor_pkey";
       public         postgres    false    200            ?
           2606    16546    Registration Registration_pkey 
   CONSTRAINT     l   ALTER TABLE ONLY public."Registration"
    ADD CONSTRAINT "Registration_pkey" PRIMARY KEY (registrationid);
 L   ALTER TABLE ONLY public."Registration" DROP CONSTRAINT "Registration_pkey";
       public         postgres    false    199            ?
           2606    16430    section Section_pkey 
   CONSTRAINT     [   ALTER TABLE ONLY public.section
    ADD CONSTRAINT "Section_pkey" PRIMARY KEY (sectionid);
 @   ALTER TABLE ONLY public.section DROP CONSTRAINT "Section_pkey";
       public         postgres    false    198            ?
           2606    16517    course course_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.course
    ADD CONSTRAINT course_pkey PRIMARY KEY (courseid);
 <   ALTER TABLE ONLY public.course DROP CONSTRAINT course_pkey;
       public         postgres    false    201            ?
           2606    16539    department department_pkey1 
   CONSTRAINT     c   ALTER TABLE ONLY public.department
    ADD CONSTRAINT department_pkey1 PRIMARY KEY (departmentid);
 E   ALTER TABLE ONLY public.department DROP CONSTRAINT department_pkey1;
       public         postgres    false    203            ?
           2606    16544    student2 student2_pkey 
   CONSTRAINT     [   ALTER TABLE ONLY public.student2
    ADD CONSTRAINT student2_pkey PRIMARY KEY (studentid);
 @   ALTER TABLE ONLY public.student2 DROP CONSTRAINT student2_pkey;
       public         postgres    false    204            ?
           2606    16534    student student_pkey 
   CONSTRAINT     Y   ALTER TABLE ONLY public.student
    ADD CONSTRAINT student_pkey PRIMARY KEY (studentid);
 >   ALTER TABLE ONLY public.student DROP CONSTRAINT student_pkey;
       public         postgres    false    202            &   }   x?M?;
?@?Zs??????&?i?ľ??`???@?Q??u|~?P??u|眑?YE?}K%???ejPEse)?ATg? ???Y?$???r
?`~?5?*??j?`???4??H??*e??a???Ll      $   q   x?M?1
?0@??>?NP??!?%S]?Wu??^?2x???!??D??F???YKx???ڗZLõ?&???3
??ND&-?D???XY?;??Ì?QE?|???og????41?      (   ?   x?u?1r?@Ek?:??8?'??r??i6?X??2????L????O?O*y?{?9?fj?!K??4?+??>?z?T,3?s?????????S??G?5?P?G>?ge?F?1*?.+???3??S??6)/??@]>???.????2??7?|J?+YHE????F?r?|???ZV?n??p?????\??ڼ?f?<P??
?zV?      *   ?   x?e?1? kx/????? ?)Ӡ???@?}?iwvF*&K?{?&?H?-?g????r?"??'?S	??.?Y?2M?Ҳ뱐??J*?{??e???u?Fb??p?
??y?H?4f??ZSă??G????}??? .?>o      '   ?   x???
?0??~L1&??????.??n?p?I??пo??90G??q_R???D?y???$av??J???????ɢ????ZhC5??O_T?P?;Ǽ????y??2vЩ?Ӈ?Q`?.????2??BD??#k      %   ?   x?M???0D???  ???.M:A ?Mpx?.J ?j???<??<??/ys|/8R???????)?H??O?J????!P4><"?.?,2?.?a?R{V?o?ע?BH??n??Z?vѮ_???????ݘ????FD_?B}      )   k   x?5̱
?@E????	;?F??"?X?Y??L?
?6?????6???-A*V?!#??l1[?vX????b?}ꑡ-{,??/??)???<G?<?xj.%?}#??L      +   I   x?3?t+?LM?4??2????4?????M,*???42??????KI	p??????9C2?s?A?=... {$?     